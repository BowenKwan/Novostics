#export LD_LIBRARY_PATH=/mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/htslib1/lib

g++ base_BAM_Prefer_end.cpp utils.cpp -o base_BAM_Prefer_end -std=c++1z -I /mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/htslib1/include/ -L /mnt/dt2_dc2stor_data202/users/data/pykwan/Novostics/htslib1/lib -lhts

./base_BAM_Prefer_end $1



















