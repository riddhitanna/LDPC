# LDPC
LDPC Codes - Encoding and Decoding
The project is coded in cpp. 
We have coded an encoder and a decoder using the concepts of Tanner graphs and Iterative Decoding for LDPC codes. 
The user can input the binary message signal that gets encoded in LDPC.
There are two channels through which the encoded message can pass through - a Binary Erasure Channel and a Binary Symmetric Channel.
Both of these are coded in separate cpp files. 
After passing throught the channels, the decoder decodes the encoded erraneous message signal. 
There is a separate file for encoding the message signal using the Product Code. 
To calculate the accuracy of the decoder for BEC and BSC channels, there are codes for the graphs.
