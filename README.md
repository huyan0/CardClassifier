# CardClassifier

Game Plan:

0. Read an isolated card from a image(by requiring the background to be back, or providing a frame for users to put the card into)

1. Segementation - isolate the number from the card

findContour, then create squares from contour

2. Feature Extraction - SIFT

3. Feature Matching - KNN implementation(?) 

  3.1 Offline - Preprocess cards, i.e. "training" samples, from this [link](https://www.kaggle.com/jamesmcguigan/playingcards?select=2D.png)

  3.2 Match real time input to one of the "training" samples
  
4. Write the result to screen
