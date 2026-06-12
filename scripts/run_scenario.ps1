# run_scenario.ps1 -- SemiOrderSystem scenario runner
#
# Usage:
#   .\scripts\run_scenario.ps1 .\scripts\scenarios\01_sample_crud.txt
#   .\scripts\run_scenario.ps1 .\scripts\scenarios\02_order_confirm_release.txt 1
#
# Args:
#   $args[0]  scenario file path (required)
#   $args[1]  ResetData: 1 = reset data/*.json before run (default 0)
#   $args[2]  ExePath: exe path (default .\x64\Debug\SemiOrderSystem.exe)
#
# Scenario file format:
#   - one line = one Enter input
#   - lines starting with '#' = comments (ignored)
#   - blank lines = Enter key (e.g. "press Enter to continue" prompt)
#   - {TODAY} = replaced with today's date (yyyyMMdd)
#   - !sleep N = wait N seconds then restart app (for timer-based tests)

$ScenarioFile = if ($args[0]) { $args[0] } else { $null }
$ResetData    = if ($args[1] -eq '1') { $true } else { $false }
$ExePath      = if ($args[2]) { $args[2] } else { ".\x64\Debug\SemiOrderSystem.exe" }

# -- Validate args -------------------------------------------------------
if (-not $ScenarioFile) {
    Write-Host "Usage: run_scenario.ps1 <scenario-file> [ResetData:0/1] [ExePath]"
    exit 1
}
if (-not (Test-Path $ExePath)) {
    Write-Host "[ERROR] Executable not found: $ExePath"
    Write-Host "        Build with Visual Studio (Debug|x64) first."
    exit 1
}
if (-not (Test-Path $ScenarioFile)) {
    Write-Host "[ERROR] Scenario file not found: $ScenarioFile"
    exit 1
}

# -- Reset data ----------------------------------------------------------
if ($ResetData) {
    [System.IO.File]::WriteAllText(".\data\samples.json",         "[]", [System.Text.Encoding]::UTF8)
    [System.IO.File]::WriteAllText(".\data\orders.json",          "[]", [System.Text.Encoding]::UTF8)
    [System.IO.File]::WriteAllText(".\data\production_jobs.json", "[]", [System.Text.Encoding]::UTF8)
    Write-Host "[RESET] data/*.json cleared" -ForegroundColor Cyan
    Write-Host ""
}

# -- Template value ------------------------------------------------------
$today = Get-Date -Format "yyyyMMdd"

# -- Parse scenario: split blocks on !sleep ------------------------------
$rawLines = [System.IO.File]::ReadAllLines($ScenarioFile, [System.Text.Encoding]::UTF8)

$blocks = [System.Collections.Generic.List[hashtable]]::new()
$buf    = [System.Collections.Generic.List[string]]::new()

foreach ($line in $rawLines) {
    if ($line -match '^\s*!sleep\s+(\d+)\s*$') {
        $blocks.Add(@{ commands = $buf.ToArray(); sleepSec = [int]$Matches[1] })
        $buf = [System.Collections.Generic.List[string]]::new()
    } else {
        $buf.Add($line)
    }
}
$blocks.Add(@{ commands = $buf.ToArray(); sleepSec = 0 })

# -- Run blocks ----------------------------------------------------------
$scenarioName = Split-Path $ScenarioFile -Leaf
Write-Host "======================================" -ForegroundColor Yellow
Write-Host " Scenario: $scenarioName"              -ForegroundColor Yellow
Write-Host "======================================" -ForegroundColor Yellow
Write-Host ""

$blockIndex = 1
foreach ($block in $blocks) {
    # strip comment lines, substitute {TODAY}, preserve blank lines (= Enter)
    $commands = $block.commands | ForEach-Object {
        $_ -replace '\{TODAY\}', $today
    } | Where-Object { $_ -notmatch '^\s*#' }

    if ($null -ne $commands -and @($commands).Count -gt 0) {
        if ($blocks.Count -gt 1) {
            Write-Host "--- [Block $blockIndex / $($blocks.Count)] ---" -ForegroundColor DarkCyan
        }

        # Write UTF-8 without BOM to temp file, redirect as stdin via cmd
        $inputText = (@($commands) -join "`r`n") + "`r`n"
        $bytes     = [System.Text.Encoding]::UTF8.GetBytes($inputText)
        $tempFile  = [System.IO.Path]::GetTempFileName()
        try {
            [System.IO.File]::WriteAllBytes($tempFile, $bytes)
            $exeAbs = (Resolve-Path $ExePath).Path
            cmd /c "`"$exeAbs`" < `"$tempFile`""
        } finally {
            Remove-Item $tempFile -Force -ErrorAction SilentlyContinue
        }
    }

    if ($block.sleepSec -gt 0) {
        Write-Host ""
        Write-Host "[TIMER] Waiting $($block.sleepSec)s for auto-complete..." -ForegroundColor Magenta
        Start-Sleep -Seconds $block.sleepSec
        Write-Host "[TIMER] Done, running next block" -ForegroundColor Magenta
        Write-Host ""
        $blockIndex++
    }
}

Write-Host ""
Write-Host "======================================" -ForegroundColor Green
Write-Host " Done: $scenarioName"                  -ForegroundColor Green
Write-Host "======================================" -ForegroundColor Green
