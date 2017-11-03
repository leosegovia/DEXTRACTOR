INSTALL INSTRUCTIONS FOR SUPER-COMPUTER 
=======================================

These are the installation instructions for Dextractor on Marylou. These instructions have been tested on RHEL 7.3.
This has been requested by Jeff Maughan 

**Install the following items:**

- git
- gcc
- gcc-c++
- make
- zlib-devel

**Download the required files:**

- HDF5
    curl -O https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-1.8.19.tar

- DEXTRACTOR
    git clone https://github.com/thegenemyers/DEXTRACTOR.git

Installation: 
-------------

**__HDF5__**

- Extract HDF5

    ```tar xvf hdf5-1.8.19.tar```

- CD into the extracted hdf5 folder

- Configure the Makefile to install hdf5 to /usr/local/hf5 folder
    
    ```./configure --prefix=/usr/local/hdf5```

- Build installation
    
    ```make```

- Install HDF5
    
    ```make install```

- Export Library path for HDF5 for the system by adding the following line to the last line of ~/.bashrc
    
    ```export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/hdf5/lib```

**__DEXTRACTOR__**

- CD into DEXTRACTOR folder

- Create LD_LIBRARY_PATH variable in the Makefile
    
    ```vim Makefile```
    (Below the second line of PATH_HDF5 paste the following)
    **LD_LIBRARY_PATH = /usr/local/hdf5/lib/libhdf5.so.10**

- Save Makefile file

- Create bin folder for user
    
    ```mkdir ~/bin```

- Build installation
    
    ```make```

- Install HDF5
    
    ```make install```

If everything completed without any errors you should be able to run the following command under ~/bin/ and get a positive return for all items:
    
    ```shell
    ldd dextractor
    ```
 
