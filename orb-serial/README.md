
## ORB Descriptors Serialization Using FileStorage.
---


	Every time compute a large iamge dataset to get Descriptos is a high overhead process, So we'd better dump all the descriptors to disk once before ,so after we read the descriptors from the file and matching them.It is a turn from CPU bound to IO bound task.


#### Is it right?? YES !
![compare](orb_serial_test.png)

