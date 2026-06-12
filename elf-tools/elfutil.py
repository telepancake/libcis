"""Minimal, dependency-free ELF reader/writer for the symbol tools.

This module parses raw ELF structures by hand so that it works uniformly
across every variant we care about:

  * ELFCLASS32 / ELFCLASS64
  * ELFDATA2LSB (little endian) / ELFDATA2MSB (big endian)
  * any machine type (x86, MIPS, RISC-V, MMIX, TileGX, ...) -- we only
    ever touch architecture-neutral structures (the section headers, the
    symbol tables and their string tables), so the e_machine value is
    irrelevant to us.
  * gcc / clang, old or new binutils -- the on-disk ELF layout is the same.

We deliberately do NOT rely on pyelftools or any external library so the
tools run anywhere a stock Python does.

Symbol/section names can in principle be arbitrary byte strings (anything
but NUL).  To round-trip them losslessly through text we always keep them
as Python ``bytes``.
"""

import struct

# --- e_ident indices / constants -----------------------------------------
EI_CLASS = 4
EI_DATA = 5
ELFCLASS32 = 1
ELFCLASS64 = 2
ELFDATA2LSB = 1
ELFDATA2MSB = 2

# --- special section indices ---------------------------------------------
SHN_UNDEF = 0
SHN_LORESERVE = 0xFF00          # >= this is a reserved index, not a real section

# --- section types -------------------------------------------------------
SHT_SYMTAB = 2
SHT_DYNSYM = 11

# --- symbol type / binding helpers ---------------------------------------
STT_SECTION = 3
STT_FILE = 4


class ElfError(Exception):
    pass


class Symbol:
    """A single symbol-table entry, plus where it lives on disk."""

    __slots__ = (
        "index", "name", "name_off", "value", "size", "info", "other",
        "shndx", "entry_off", "strtab_off",
    )

    def __init__(self, index, name, name_off, value, size, info, other,
                 shndx, entry_off, strtab_off):
        self.index = index            # index within its symbol table
        self.name = name              # bytes: the symbol name (b'' if none)
        self.name_off = name_off      # st_name: offset into the string table
        self.value = value
        self.size = size
        self.info = info
        self.other = other
        self.shndx = shndx            # st_shndx
        self.entry_off = entry_off    # absolute file offset of this entry
        self.strtab_off = strtab_off  # absolute file offset of the strtab

    @property
    def bind(self):
        return self.info >> 4

    @property
    def type(self):
        return self.info & 0xF

    @property
    def defined(self):
        return self.shndx != SHN_UNDEF


class SymbolTable:
    """A symbol table section paired with its string-table section."""

    __slots__ = ("name", "is_dynamic", "symbols",
                 "strtab_off", "strtab_size")

    def __init__(self, name, is_dynamic, symbols, strtab_off, strtab_size):
        self.name = name              # bytes: section name (e.g. b'.symtab')
        self.is_dynamic = is_dynamic  # True for .dynsym
        self.symbols = symbols        # list[Symbol]
        self.strtab_off = strtab_off  # absolute file offset of string table
        self.strtab_size = strtab_size


class Elf:
    """Parsed view of one ELF file.  Keeps the raw bytes for editing."""

    def __init__(self, data, path=None):
        self.data = bytearray(data)
        self.path = path
        self._parse()

    # -- low level numeric access -----------------------------------------
    def _u(self, fmt, off):
        return struct.unpack_from(self.end + fmt, self.data, off)[0]

    def _parse(self):
        d = self.data
        if len(d) < 64 or d[0:4] != b"\x7fELF":
            raise ElfError("not an ELF file")

        ei_class = d[EI_CLASS]
        ei_data = d[EI_DATA]
        if ei_class == ELFCLASS32:
            self.is64 = False
        elif ei_class == ELFCLASS64:
            self.is64 = True
        else:
            raise ElfError("unknown ELF class %d" % ei_class)

        if ei_data == ELFDATA2LSB:
            self.end = "<"
        elif ei_data == ELFDATA2MSB:
            self.end = ">"
        else:
            raise ElfError("unknown ELF data encoding %d" % ei_data)

        # Section header table location, from the ELF header.
        if self.is64:
            self.e_shoff = self._u("Q", 0x28)
            self.e_shentsize = self._u("H", 0x3A)
            self.e_shnum = self._u("H", 0x3C)
        else:
            self.e_shoff = self._u("I", 0x20)
            self.e_shentsize = self._u("H", 0x2E)
            self.e_shnum = self._u("H", 0x30)

        if self.e_shoff == 0 or self.e_shnum == 0:
            # Stripped of section headers entirely: no symbol tables to touch.
            self.sections = []
            self.symtabs = []
            return

        self._parse_sections()
        self._parse_symtabs()

    def _parse_sections(self):
        secs = []
        for i in range(self.e_shnum):
            off = self.e_shoff + i * self.e_shentsize
            if self.is64:
                sh_name = self._u("I", off + 0x00)
                sh_type = self._u("I", off + 0x04)
                sh_link = self._u("I", off + 0x28)
                sh_offset = self._u("Q", off + 0x18)
                sh_size = self._u("Q", off + 0x20)
                sh_entsize = self._u("Q", off + 0x38)
            else:
                sh_name = self._u("I", off + 0x00)
                sh_type = self._u("I", off + 0x04)
                sh_link = self._u("I", off + 0x18)
                sh_offset = self._u("I", off + 0x10)
                sh_size = self._u("I", off + 0x14)
                sh_entsize = self._u("I", off + 0x24)
            secs.append({
                "name_off": sh_name, "type": sh_type, "link": sh_link,
                "offset": sh_offset, "size": sh_size, "entsize": sh_entsize,
            })
        self.sections = secs

    def _cstr(self, base, off):
        """Read a NUL-terminated string at base+off, return bytes."""
        start = base + off
        end = self.data.find(b"\x00", start)
        if end == -1:
            end = len(self.data)
        return bytes(self.data[start:end])

    def _parse_symtabs(self):
        self.symtabs = []
        for sec in self.sections:
            if sec["type"] not in (SHT_SYMTAB, SHT_DYNSYM):
                continue
            link = sec["link"]
            if link == 0 or link >= len(self.sections):
                continue
            strsec = self.sections[link]
            strtab_off = strsec["offset"]
            strtab_size = strsec["size"]

            entsize = sec["entsize"]
            if entsize == 0:
                entsize = 24 if self.is64 else 16
            count = sec["size"] // entsize if entsize else 0

            secname = self._cstr(self._shstrtab_off(), sec["name_off"]) \
                if self._shstrtab_off() is not None else b""

            symbols = []
            for i in range(count):
                eoff = sec["offset"] + i * entsize
                if self.is64:
                    st_name = self._u("I", eoff + 0x00)
                    st_info = self.data[eoff + 0x04]
                    st_other = self.data[eoff + 0x05]
                    st_shndx = self._u("H", eoff + 0x06)
                    st_value = self._u("Q", eoff + 0x08)
                    st_size = self._u("Q", eoff + 0x10)
                else:
                    st_name = self._u("I", eoff + 0x00)
                    st_value = self._u("I", eoff + 0x04)
                    st_size = self._u("I", eoff + 0x08)
                    st_info = self.data[eoff + 0x0C]
                    st_other = self.data[eoff + 0x0D]
                    st_shndx = self._u("H", eoff + 0x0E)
                name = self._cstr(strtab_off, st_name) if st_name else b""
                symbols.append(Symbol(
                    index=i, name=name, name_off=st_name, value=st_value,
                    size=st_size, info=st_info, other=st_other,
                    shndx=st_shndx, entry_off=eoff, strtab_off=strtab_off,
                ))
            self.symtabs.append(SymbolTable(
                name=secname, is_dynamic=(sec["type"] == SHT_DYNSYM),
                symbols=symbols, strtab_off=strtab_off,
                strtab_size=strtab_size,
            ))

    def _shstrtab_off(self):
        # e_shstrndx -> section holding section-header names.  Optional; only
        # used to label tables for messages, never for correctness.
        if getattr(self, "_shstr_cached", "missing") != "missing":
            return self._shstr_cached
        try:
            if self.is64:
                e_shstrndx = self._u("H", 0x3E)
            else:
                e_shstrndx = self._u("H", 0x32)
            if e_shstrndx < len(self.sections):
                self._shstr_cached = self.sections[e_shstrndx]["offset"]
            else:
                self._shstr_cached = None
        except Exception:
            self._shstr_cached = None
        return self._shstr_cached

    # -- editing ----------------------------------------------------------
    def overwrite_string(self, strtab_off, name_off, new_name):
        """Overwrite the NUL-terminated string at strtab_off+name_off.

        ``new_name`` (bytes) plus its NUL must fit in the space occupied by
        the original string plus its NUL, so that nothing else in the file
        moves.  Returns the number of bytes the string shrank by.
        """
        start = strtab_off + name_off
        old_end = self.data.find(b"\x00", start)
        if old_end == -1:
            raise ElfError("unterminated string in string table")
        old_len = old_end - start
        if len(new_name) > old_len:
            raise ElfError(
                "new name %r longer than original %d bytes"
                % (new_name, old_len))
        self.data[start:start + len(new_name)] = new_name
        # NUL-terminate the new (shorter) name; leave the rest as it was,
        # it becomes unreferenced dead bytes inside the string table.
        self.data[start + len(new_name)] = 0
        return old_len - len(new_name)

    def write(self, path=None):
        path = path or self.path
        with open(path, "wb") as f:
            f.write(self.data)


def load(path):
    with open(path, "rb") as f:
        return Elf(f.read(), path=path)
