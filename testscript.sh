#!/bin/bash
make

test_pgmEcho_noFilesProvided(){
    output=$(./pgmEcho)
    if [ "$output" = "Usage: ./pgmEcho inputImage.pgm outputImage.pgm" ]; then
        echo "PASSED pgmEcho UsageStringTest"
    else
        echo "FAILED pgmEcho UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgmComp_noFilesProvided(){
    output=$(./pgmComp)
    if [ "$output" = "Usage: ./pgmComp inputImage.pgm inputImage.pgm" ]; then
        echo "PASSED pgmComp UsageStringTest"
    else
        echo "FAILED pgmComp UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgmb2a_noFilesProvided(){
    output=$(./pgmb2a)
    if [ "$output" = "Usage: ./pgmb2a inputImage.pgm outputImage.pgm" ]; then
        echo "PASSED pgmb2a UsageStringTest"
    else
        echo "FAILED pgmb2a UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgma2b_noFilesProvided(){
    output=$(./pgma2b)
    if [ "$output" = "Usage: ./pgma2b inputImage.pgm outputImage.pgm" ]; then
        echo "PASSED pgmb2a UsageStringTest"
    else
        echo "FAILED pgmb2a UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgmReduce_noFilesProvided(){
    output=$(./pgmReduce)
    if [ "$output" = "Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm" ]; then
        echo "PASSED pgmReduce UsageStringTest"
    else
        echo "FAILED pgmReduce UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgmTile_noFilesProvided(){
    output=$(./pgmTile)
    if [ "$output" = "Usage: ./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm" ]; then
        echo "PASSED pgmTile UsageStringTest"
    else
        echo "FAILED pgmTile UsageStringTest"
    fi;
    echo -e "\n"
}

test_pgmAssemble_noFilesProvided(){
    output=$(./pgmAssemble)
    if [ "$output" = "Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+n" ]; then
        echo "PASSED pgmAssemble UsageStringTest"
    else
        echo "FAILED pgmAssemble UsageStringTest"
    fi;
    echo -e "\n"
}

# Tests for functionality

test_pgmEcho_binaryInput(){
    output=$(./pgmEcho pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFileECHOED.pgm)
    cmp --silent pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFileECHOED.pgm && echo 'PASSED pgmEcho Echoed Binary File identical with Original File' || echo 'FAILED pgmEcho Echoed Binary File identical with Original File'
    echo -e "\tRemoved generated pgm Binary File \n"
    rm pgm/pgmTemporaryOutputStorage/BinaryFileECHOED.pgm
}

test_pgmEcho_ASCIInput(){
    output=$(./pgmEcho pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTemporaryOutputStorage/ASCIIFileECHOED.pgm)
    cmp --silent pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTemporaryOutputStorage/ASCIIFileECHOED.pgm && echo 'PASSED pgmEcho Echoed ASCII File identical with Original File' || echo 'FAILED pgmEcho Echoed ASCII File identical with Original File'
    echo -e "\tRemoved generated pgm ASCII File \n"
    rm pgm/pgmTemporaryOutputStorage/ASCIIFileECHOED.pgm
}

test_pgmComp_ASCIIWithASCIInput(){
    output=$(./pgmComp pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTestFiles/ASCIIFile.pgm)
    if [ "$output" = "IDENTICAL	pgmComp	The two files were identical" ]; then
        echo "PASSED pgmComp ASCII to ASCII comparison"
    else
        echo "FAILED pgmComp ASCII to ASCII comparison"
    fi;
    echo -e "\n"
}

test_pgmComp_BinaryWithBinaryInput(){
    output=$(./pgmComp pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTestFiles/BinaryFile.pgm)
    if [ "$output" = "IDENTICAL	pgmComp	The two files were identical" ]; then
        echo "PASSED pgmComp Binary to Binary comparison"
    else
        echo "FAILED pgmComp Binary to Binary comparison"
    fi;
    echo -e "\n"
}

test_pgmComp_BinaryWithASCIIInput(){
    output=$(./pgmb2a pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFileConvertedToASCII.pgm)
    output=$(./pgmComp pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFileConvertedToASCII.pgm)
    if [ "$output" = "IDENTICAL	pgmComp	The two files were identical" ]; then
        echo "PASSED pgmComp Identical Binary to ASCII comparison"
    else
        echo "FAILED pgmComp Identical Binary to ASCII comparison"
    fi;
    echo -e "\n"
}

test_pgmReduce_BinaryInput(){
    output=$(./pgmReduce pgm/pgmTestFiles/BinaryFile.pgm 10 pgm/pgmTemporaryOutputStorage/BinaryFileREDUCED.pgm)
    cmp --silent pgm/pgmTestFiles/BinaryFileReducedFactor10.pgm pgm/pgmTemporaryOutputStorage/BinaryFileREDUCED.pgm && echo 'PASSED pgmReduce Reduced Binary Files' || echo 'FAILED pgmReduce Reduced Binary Files'
    echo -e "\tRemoved generated pgm Binary File \n"
    rm pgm/pgmTemporaryOutputStorage/BinaryFileREDUCED.pgm
}

test_pgmReduce_ASCIIInput(){
    output=$(./pgmReduce pgm/pgmTestFiles/ASCIIFile.pgm 2 pgm/pgmTemporaryOutputStorage/ASCIIFileREDUCED.pgm)
    cmp --silent pgm/pgmTestFiles/ASCIIFileReducedFactor2.pgm pgm/pgmTemporaryOutputStorage/ASCIIFileREDUCED.pgm && echo 'PASSED pgmReduce Reduced ASCII Files' || echo 'FAILED pgmReduce Reduced ASCII Files'
    echo -e "\tRemoved generated pgm ASCII File \n"
    rm pgm/pgmTemporaryOutputStorage/ASCIIFileREDUCED.pgm
}

test_pgma2bAndpgmb2a_BinaryInput(){
    output=$(./pgmb2a pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII.pgm)
    output=$(./pgma2b pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII.pgm pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII2Binary.pgm)
    cmp --silent pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII2Binary.pgm && echo 'PASSED pgmb2a&pgma2b Converting Binary to ASCII and back' || echo 'FAILED pgmb2a&pgma2b Converting Binary to ASCII and back'
    echo -e "\tRemoved generated pgm Binary File \n"
    rm pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII2Binary.pgm
    rm pgm/pgmTemporaryOutputStorage/BinaryFile2ASCII.pgm
}

test_pgma2bAndpgmb2a_ASCIIInput(){
    output=$(./pgma2b pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary.pgm)
    output=$(./pgmb2a pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary.pgm pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary2ASCII.pgm)
    cmp --silent pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary2ASCII.pgm && echo 'PASSED pgmb2a&pgma2b Converting ASCII to Binary and back' || echo 'PASSED pgmb2a&pgma2b Converting ASCII to Binary and back'
    echo -e "\tRemoved generated pgm ASCII File \n"
    rm pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary2ASCII.pgm
    rm pgm/pgmTemporaryOutputStorage/ASCIIFile2Binary.pgm
}

test_pgmTileAndpgmAssemble_BinaryInput(){
  output=$(./pgmTile pgm/pgmTestFiles/BinaryFile.pgm 2 pgm/pgmTemporaryOutputStorage/BinaryFileTILED'<row>'_'<column>'.pgm)
  output=$(./pgmAssemble pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled.pgm 640 426 0 0 pgm/pgmTemporaryOutputStorage/BinaryFileTILED0_0.pgm 0 320 pgm/pgmTemporaryOutputStorage/BinaryFileTILED0_1.pgm 213 0 pgm/pgmTemporaryOutputStorage/BinaryFileTILED1_0.pgm 213 320 pgm/pgmTemporaryOutputStorage/BinaryFileTILED1_1.pgm)
  output=$(./pgma2b pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled.pgm pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled2Binary.pgm)
  cmp --silent pgm/pgmTestFiles/BinaryFile.pgm pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled2Binary.pgm && echo 'PASSED pgmTile&pgmAssemble Tiling Binary Image and Assembling it back up' || echo 'FAILED pgmTile&pgmAssemble Tiling Binary Image and Assembling it back up'
  echo -e "\tRemoved generated pgm Binary File \n"
  rm pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled2Binary.pgm
  rm pgm/pgmTemporaryOutputStorage/BinaryFileTiledAndAssembled.pgm
  rm pgm/pgmTemporaryOutputStorage/*.pgm
}

test_pgmTileAndpgmAssemble_ASCIIInput(){
  output=$(./pgmTile pgm/pgmTestFiles/ASCIIFile.pgm 2 pgm/pgmTemporaryOutputStorage/ASCIIFileTILED'<row>'_'<column>'.pgm)
  output=$(./pgmAssemble pgm/pgmTemporaryOutputStorage/ASCIIFileTiledAndAssembled.pgm 24 8 0 0 pgm/pgmTemporaryOutputStorage/ASCIIFileTILED0_0.pgm 0 12 pgm/pgmTemporaryOutputStorage/ASCIIFileTILED0_1.pgm 4 0 pgm/pgmTemporaryOutputStorage/ASCIIFileTILED1_0.pgm 4 12 pgm/pgmTemporaryOutputStorage/ASCIIFileTILED1_1.pgm)
  cmp --silent pgm/pgmTestFiles/ASCIIFile.pgm pgm/pgmTemporaryOutputStorage/ASCIIFileTiledAndAssembled.pgm && echo 'PASSED pgmTile&pgmAssemble Tiling ASCII Image and Assembling it back up' || echo 'FAILED pgmTile&pgmAssemble Tiling ASCII Image and Assembling it back up'
  echo -e "\tRemoved generated pgm ASCII File \n"
  rm pgm/pgmTemporaryOutputStorage/ASCIIFileTiledAndAssembled.pgm
  rm pgm/pgmTemporaryOutputStorage/*.pgm
}


test_pgmEcho_noFilesProvided
test_pgmComp_noFilesProvided
test_pgmb2a_noFilesProvided
test_pgma2b_noFilesProvided
test_pgmReduce_noFilesProvided
test_pgmAssemble_noFilesProvided
test_pgmTile_noFilesProvided
test_pgmEcho_binaryInput
test_pgmEcho_ASCIInput
test_pgmComp_ASCIIWithASCIInput
test_pgmComp_BinaryWithBinaryInput
test_pgmComp_BinaryWithASCIIInput
test_pgmReduce_BinaryInput
test_pgmReduce_ASCIIInput
test_pgma2bAndpgmb2a_BinaryInput
test_pgma2bAndpgmb2a_ASCIIInput
test_pgmTileAndpgmAssemble_BinaryInput
test_pgmTileAndpgmAssemble_ASCIIInput
make clean
