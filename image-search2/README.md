



### Use a image in every group as query image, search the dataset and get the score , The image dataset has 56 groups , every group has 4 images.SO, We will see if the retrieved top 4 images is this 4 images, so the possible score is 1,2,3,4.


### Test It!

g++ -std=c++11 -c orb.cpp `pkg-config opencv --cflags --libs`

g++ -o search main.cpp orb.o `pkg-config opencv --cflags --libs` 

./search 

```
Score For 132 = 4
Score For 136 = 3
Score For 140 = 3
Score For 144 = 4
Score For 148 = 4
Score For 152 = 4
Score For 156 = 4
Score For 160 = 4
Score For 164 = 3
Score For 168 = 4
Score For 172 = 3 (40)
Score For 176 = 4
Score For 180 = 4
Score For 184 = 4
Score For 188 = 4
Score For 192 = 3
Score For 196 = 1
Score For 200 = 4
Score For 204 = 1
Score For 208 = 4
Score For 212 = 2
Score For 216 = 4
Score For 220 = 3
Score For 224 = 2 (40)
Score For 228 = 4
Score For 232 = 4
Score For 236 = 3
Score For 240 = 4
Score For 244 = 4
Score For 248 = 4
Score For 252 = 2
Score For 256 = 3
Score For 260 = 2
Score For 264 = 4
Score For 268 = 3
Score For 272 = 2
Score For 276 = 4
Score For 280 = 1
Score For 284 = 3
Score For 288 = 4
Score For 292 = 3
Score For 296 = 3 (57)
Score For 300 = 4
Score For 304 = 4
Score For 308 = 4
Score For 312 = 4
Score For 316 = 3
Score For 320 = 3
Score For 324 = 3
Score For 328 = 4
Score For 332 = 3
Score For 336 = 4
Score For 340 = 3
Score For 344 = 4
Score For 348 = 1
Score For 352 = 4 (48)
Average Score For This Dataset: 3.30357

```

