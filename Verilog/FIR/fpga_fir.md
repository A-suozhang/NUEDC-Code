# Implement FPGA with FIR CORE
## Using MATLAB FDATOOL
* Design The Filter 
    * Be Careful to Quantize The Coefficients as Fixed Points 
    * Export The COE File
## Testing It In Matlab
1. in FDA Tool - Export to Workplace - 
```
y_out = filter(COEFFICENTS , 1, y_in);
```
2. in FDA Tool - Gnenrate Matlab Code
```
```
## In IPCORE Setting Import The certain COE File
