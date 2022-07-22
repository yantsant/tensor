# tensor
Classes Tensor and Vector provides basic tensor-vector calculus in N dimensional spatial

Classes Tensor and Vector provide +/-/scal product operators between each other. So you should not keep i nmind at how basis are component of current tensor/vector, 
internal functionality does operation correctly.

Using shared_ptr for manage basis object allows to have many objects (Tensor/Vector) at the same basis object. 
So all objects linked to the one basis will change at the same time when basis will be changed.
Basis object will live till the last Tensor/Vector object will be destroyed.
