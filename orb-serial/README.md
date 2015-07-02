
## ORB Descriptors Serialization Using FileStorage.



Every time compute a large iamge dataset to get Descriptos is a high overhead process, So we'd better dump all the descriptors to disk once before ,so after we read the descriptors from the file and matching them.It is a turn from CPU bound to IO bound task.


#### Is it right?? YES !
![compare](orb_serial_test.png)



=>  orb_write.cpp : Extract ORB descriptors of a image and dump to a file! 

=>  orb_write_batch.cpp : Extract ORB descriptors of image dataset and dump them  to a coresponding files! **very useful**....

