# =================================================================
# SCRIPT TO FIND FOLDERS CONTAINING .PEX FILES
# =================================================================

# 1. DEFINE THE FOLDER TO SCAN
# !! IMPORTANT: Replace the placeholder path below with your actual target folder path.
$rootPath = "E:\Lorerim\mods"

# 2. SCAN THE TARGET FOLDER
Write-Host "Scanning folder: '$rootPath' for subfolders containing .pex files..." -ForegroundColor Cyan

# This command finds all '.pex' files and groups them by their parent directory.
# Select-Object -Unique ensures that each folder is listed only once.


Get-ChildItem -Path $rootPath -Recurse -Directory | Where-Object {
    Get-ChildItem -Path $_.FullName -Filter *.pex -File -ErrorAction SilentlyContinue
} | Select-Object -ExpandProperty FullName


Write-Host "`nScan complete." -ForegroundColor Green