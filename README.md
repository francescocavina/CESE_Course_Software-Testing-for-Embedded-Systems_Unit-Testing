## About this repository

This respository is used for a practical work from the Software Testing for Embedded Systems course (2023). This course belongs to the Master's Degree in Embedded Systems at the Universidad de Buenos Aires (UBA).

Here Unit Testing is applied to the UAI (User Application Interface) part of a driver for the FS-A8S radio control receiver from FlySky. For more information see [FlySky FS-i6X radio control model](https://www.flysky-cn.com/i6x-gaishu-1).

Note: This driver is dived in to main parts: User Application Interface (UAI) for the high-level abstraction layer part and Hardware Interface (HWI) for the low-level abstraction layer part. The first part is tested here.

Moreover, this driver is part of my Master's Degree thesis "Development of a Quadcopter Flight Controller".

On the other hand, this unit testing is applied using Ceedling. For more information see [Ceedling](http://www.throwtheswitch.org/ceedling). This framework uses:
- **Unity** — Curiously Powerful Unit Testing in C for C.
- **CMock** — Automagical generation of stubs and mocks for Unity Tests.
- **FFF**   — Fake Function Framework for generation of mocks.

### To take into account:
1. Only one mock has been employed to replace the original function definition in the low-level abstraction layer part of the driver.
2. Data type mock has been defined and can be found in the "mock_data.h" file. This is because orginally, the driver used the UART_HandleTypeDef data type which is located in the HAL (Hardware Abstraction Layer) library of STM and there are many dependencies.
3. 6 Test Cases have been implemented to test the two public functions available in the tested source file.
4. To run the test, you should type in the terminal: `ceedling`
