import os

os.system("g++ src/axios.cpp -o build/axios -std=c++17 -O2 -lcpr -lcurl")
os.system("climapp/build/axios")