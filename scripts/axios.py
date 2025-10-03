import os

os.system("g++ climapp/src/axios.cpp -o climapp/build/axios -std=c++17 -O2")
os.system("climapp/build/axios")