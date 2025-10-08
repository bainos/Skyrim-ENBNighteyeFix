# Define the target directories for clarity and easy reference
$target1 = "C:\Users\b4ino\Documents\My Games\Skyrim Special Edition"
$target2 = "E:\Lorerim"
$target3 = "E:\Lorerim\mods\Rudy ENB\SKSE\plugins"

# --- Function to check log existence and content (simplifies logic) ---
function Check-LogStatus {
    param(
        [string]$Path,
        [string]$LogType,
        [string]$Filter,
        [string]$SearchTerm
    )
    Write-Host "`n📝 Checking for $LogType logs..." -ForegroundColor Yellow
    
    # Get files strictly from the path
    $logs = Get-ChildItem -Path $Path -Recurse -File -Filter $Filter -ErrorAction SilentlyContinue
    
    if ($logs) {
        Write-Host "Found $LogType logs:" -ForegroundColor Yellow
        $logs | Select-Object FullName, Name, LastWriteTime | Out-Host

        # Search for 'Bainos'
        Write-Host "`n🔍 Searching for '$SearchTerm' in $LogType logs..." -ForegroundColor Green
        $searchResults = $logs | Select-String -Pattern $SearchTerm -CaseSensitive:$false -ErrorAction SilentlyContinue
        
        if ($searchResults) {
            $searchResults | ForEach-Object {
                "$($_.Path):$($_.LineNumber): $($_.Line)"
            } | Out-Host
        } else {
            Write-Host "No '$SearchTerm' found in $LogType logs." -ForegroundColor DarkYellow
        }
    } else {
        Write-Host "No $LogType logs found." -ForegroundColor DarkYellow
    }
}

# --- Checking Target 1: Skyrim Special Edition Directory ($target1) ---

Write-Host "`n=================================================" -ForegroundColor Cyan
Write-Host "Checking: $target1" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

# 1-4. Papyrus and SKSE logs for $target1
Check-LogStatus -Path $target1 -LogType "papyrus" -Filter "Papyrus.0.log" -SearchTerm "Bainos"
Check-LogStatus -Path $target1 -LogType "SKSE" -Filter "skse*.log" -SearchTerm "Bainos"


# --- Checking Target 2: Lorerim Directory ($target2) ---

Write-Host "`n=================================================" -ForegroundColor Cyan
Write-Host "Checking: $target2" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

# 5-8. Papyrus and SKSE logs for $target2
Check-LogStatus -Path $target2 -LogType "papyrus" -Filter "Papyrus.0.log" -SearchTerm "Bainos"
Check-LogStatus -Path $target2 -LogType "SKSE" -Filter "skse*.log" -SearchTerm "Bainos"


# --- Checking Target 3: Plugins Directory ($target3 - Only 'bainos*' files) ---

Write-Host "`n=================================================" -ForegroundColor Cyan
Write-Host "Checking: $target3" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

# 9. Search for 'bainos*' files in plugins directory (Target 3)
Write-Host "`n🔎 Searching for 'bainos*' files in plugins..." -ForegroundColor Magenta
$bainosFiles = Get-ChildItem -Path $target3 -Recurse -File -Filter "bainos*" -ErrorAction SilentlyContinue

if ($bainosFiles) {
    $bainosFiles | Select-Object FullName, Name, LastWriteTime | Out-Host
} else {
    Write-Host "No 'bainos*' files found in plugins." -ForegroundColor DarkYellow
}