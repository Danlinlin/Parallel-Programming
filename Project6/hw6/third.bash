for a in 32 128 256
do
        for b in 1 512 1024 2048 4096 8192
        do
                g++ -DNMB=$b -DLOCAL_SIZE=$a -o third third.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
                ./third 2>> third.csv

        done
done
