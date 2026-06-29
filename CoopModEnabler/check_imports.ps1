$path = (Get-Item "e:\Mods\以撒的结合\Isaac-Online-Modded\reference\isaac-ng.exe").FullName
$fs = [System.IO.File]::OpenRead($path)
$br = New-Object System.IO.BinaryReader($fs)

# Read DOS header
$magic = $br.ReadUInt16()  # 'MZ' = 0x5A4D
$fs.Position = 0x3C
$peOffset = $br.ReadInt32()

# Seek to PE header
$fs.Position = $peOffset
$peSig = $br.ReadInt32()  # 'PE\0\0' = 0x4550

# Read COFF header
$machine = $br.ReadUInt16()

$br.Close()
$fs.Close()

switch ($machine) {
    0x14C  { Write-Host "Machine: i386 (32-bit x86)" }
    0x8664 { Write-Host "Machine: AMD64 (64-bit x64)" }
    0xAA64 { Write-Host "Machine: ARM64" }
    default { Write-Host "Machine: Unknown (0x$($machine.ToString('X4')))" }
}
