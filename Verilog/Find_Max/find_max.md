# Find MAX Module
* This Module Is Used For Finding The Max From Streaming Data Coming From a "PINGPONG RAM"
## The PIGPONG RAM
* This Module Is Self Built, Applying Ping Pong Mechanism On A Regular Block RAM
    * Used Xilinx IP Core - BLock RAM
    * True Dual Port RAM, The ADDR line is 1 bit wilder Than Tne ping pong ram module
    * Has 2 Port, A & B, A is used To Write, B is used to read
    * When readya/readyb Is High, means that this port could be accessed
    * WHen Firstly Start Working, The Other Port's Ready Stay LOW, When Finishing One Process Of Writing/Reading, Raise finisha/finishb for **One Cycle**, Then The Other Port's Ready will be HIGH again.Then Ready Could be HIGH
    * If you Wanna Read The RAM(initialized Form .COE), You should First raise finisha HIGH, Then Read It
## The Find_MAX Module
* used For Finding The MAx From Streaming Data Loaded From The RAM, returning The Max Value & Its ADDR, You can read It When Valid Is HIGH
* If There Are Multiple MAXs, The Valid Signal Will Be Raised For More Than 1 Cycle. (The Max Value Will Stay The Same, But The ADDR Comes At A Sream)
* The ENA Signal is **IMPORTANT**, Denotes That The Input Data Is Valid.
    * IN PINGPONGRAM's Case, The Data & The ADDR_R2(addr latch 2 cycle)
    * if ena is wrong, more data will be added to Compare, So be careful
* After Finishing Each Reading, The Finishb will Be Raised, The PINGPONGRAM will Be Pointed At Another Sector. **MAKE SURE YOUR ENA SIGNAL STAY HIGH DURING READING FROM ONE SECTOR OF PINGPONGRAM**
