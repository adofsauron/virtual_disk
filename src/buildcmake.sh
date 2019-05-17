#build base
cd base
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(base  ">>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..


#build client
cd client
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(client  ">>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..

#build cmd
cd cmd
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(cmd  ">>CMakeLists.txt
ls cmd_generalize/*.cpp>>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..

#build handle
cd handle
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(handle  ">>CMakeLists.txt
ls file_sys/*.cpp>>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..

#build tools
cd tools
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(tools  ">>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..

#build start
cd start
rm -f CMakeLists.txt

echo "# CMakeLists file" >>CMakeLists.txt
echo "">>CMakeLists.txt
echo "add_library(start  ">>CMakeLists.txt
ls *.cpp>>CMakeLists.txt
echo  ") ">>CMakeLists.txt

cd ..