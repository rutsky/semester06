#!/bin/bash

# build_fw.sh
# Script for building firmware with hacks.
# Vladimir Rutsky <altsysrq@gmail.com>
# 15.03.2009

set -e

FWfixToolFile="../fw_correct_header/fw_correct_header"
origFWFile="FW_UB890_1.15.rom"
baseVersionStr="1.15" # 4 bytes
tempFWFile=".fw_tmp"
tempFWFileDst=".fw_tmp2"
versionFile="fw_version"
buildLockFile=".lock"

outputFWFile="FW_UB890.rom"
outputFontFile="FontData.FNT"

if [ -e $buildLockFile ]; then
  # Lock file exists.
  echo "Building process is locked. Remove '$buildLockFile' file." 
  exit 0
fi

increase_version()
{
  version=`cat $versionFile`
  echo -n "Increasing version from '"$version"' "  #debug
  version=$(($version + 1))
  echo "to '"$version"'." #debug
  echo $version > $versionFile
}

inject_file()
{
  fileName=$1
  pos=$2
  echo "Injecting '"$fileName"' into position "`printf "0x%08x" $pos` #debug
  
  dd if=$tempFWFile of=$tempFWFileDst bs=1 count=$pos status=noxfer
  cat $fileName >> $tempFWFileDst
  dd if=$tempFWFile bs=1 skip=$(($pos + `stat -c%s $fileName`)) status=noxfer >> $tempFWFileDst
  mv $tempFWFileDst $tempFWFile
}

inject_fw_reset_version_str()
{
  fwResetVersionStrTmpFile=".fwResetVersionStrTmpFile"
  fwResetVersionStrOffset=$((0x00029ff7))

  printf "#%04X" $version > $fwResetVersionStrTmpFile
  
  fwResetVersionStr=`cat $fwResetVersionStrTmpFile`
  
  echo "Writing firmware reset version string: '"$fwResetVersionStr"'."
  inject_file $fwResetVersionStrTmpFile $fwResetVersionStrOffset
  
  rm -f $fwResetVersionStrTmpFile
}

inject_version_str()
{
  versionStrTmpFile=".versionStrTmpFile"
  versionStrOffset=$((0x0000a014))

  # 16 bytes max with zero.
  printf "Bob #%04X %4s\0" $version $baseVersionStr > $versionStrTmpFile
  
  versionStr=`cat $versionStrTmpFile`

  echo "Writing version string: '"$versionStr"'."
  inject_file $versionStrTmpFile $versionStrOffset
  
  rm -f $versionStrTmpFile
}

inject_date_str()
{
  dateStrTmpFile=".dateStrTmpFile"
  dateStrOffset=$((0x00009ffc))
 
  # Note: time zone is not included.
  date "+%F %H:%M:%S" | tr -d '\n' > $dateStrTmpFile
  printf "\0" >> $dateStrTmpFile
  
  dateStr=`cat $dateStrTmpFile`
  
  echo "Writing date string: '"$dateStr"'."
  inject_file $dateStrTmpFile $dateStrOffset
  
  rm -f $dateStrTmpFile
}

append_func()
{
  # Adding function code
  cat func.bin >> $tempFWFile
}

inject_jump()
{
  jumpOffset=$((0x9d08))

  # Adding function code
  inject_file jump.bin $jumpOffset
}

# Increasing version first.
increase_version

# Current version.
version=`cat $versionFile`

# Making copy of original firmware.
cp $origFWFile $tempFWFile
chmod 0644 $tempFWFile

# Injecting version number.
inject_fw_reset_version_str
inject_version_str
inject_date_str
append_func
inject_jump

# Fixing control sum.
$FWfixToolFile $tempFWFile $tempFWFileDst
mv $tempFWFileDst $tempFWFile

# Renaming hacked firmware to result file.
mv $tempFWFile $outputFWFile

# Locking building process.
#touch $buildLockFile # creating lock file
