The "$(CMSIS_name).h" and "system_$(CMSIS_name).h" files are provided 
only as a functional sample.

For real applications they must be replaced by the vendor provided files.

Extensions to the ARM CMSIS files:

- the file "cmsis_device.h" was added, as a portable method to include the 
vendor device header file in library sources. 

  