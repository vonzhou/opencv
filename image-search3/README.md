



### As [image-serch2], this time the train images's descriptors  are read from the file,(Q: is it faster ?)

### Yes, It is Faster, but My question is : the descriptors computing is not the bottleneck, the matching is. Why This can make it faster ???


### Test It! Compare !


![image](search3-1.png)
![image](search3-2.png)


### SO, This time Use 1000 images as the dataset to see how query image size impact the retrieval score!

| Scale Factor   |  Retrieval Score    |  
| --------  | :-----:    |
|1 		|  3.156 | 
|0.95 		|  3.152 | 
|0.9  |  3.152 | 
|0.85  |  3.124 | 
|0.8  |  3.056 | 
|0.75  |  3.04 | 
| 0.7 | 3.04 |
| 0.65 | 2.944 |
| 0.6 | 2.884 |
| 0.55 | 2.812 |
| 0.5 | 2.728 |
| 0.45 |  2.6|
| 0.4 |  2.452|
| 0.35 |  2.252 |     
| 0.3 | 2.024 |     
| 0.25 | 1.612 |     
| 0.2 | 0.2|     
| <=0.1| 0	|

