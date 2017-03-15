# cvtest
My OpenCV project for testing any ideas.
## introduction

- **ClearBorder.cpp**: 
The function **ClearBorder(Mat src, Mat &dst)** fills light structures connected to the image border like **imclearboder()** in **Matlab**. 
Check parameters of **cv::floodFill()** if you want to apply it. Be aware that if the size of src is m×n, the size of dst will be (m+2)×(n+2).  
Updated on 16th Oct.  



- **ColorDeconv.cpp**:   
The function implements H-E stain separation using Ruifrok and Johnston's method described in [1].

Reference:
[1] Ruifrok AC, Johnston DA. Quantification of histochemical staining by color deconvolution. Anal Quant Cytol Histol 23: 291-299, 2001.

