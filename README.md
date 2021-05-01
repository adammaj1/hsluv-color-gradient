
# Examples
* General Type
  * continous  
  * discrete  
* Joining type 
  * no  
  * steps  
  * tubes  
  * diverging  

## HSV = [0, 100, v]


### continous
![](./images/0_100_continous_no.png  "description")   

![](./images/0_100_continous_no_2D.png  "description")   




![](./images/0_100_continous_steps.png "description")   

![](./images/0_100_continous_steps_2D.png "description")   


![](./images/0_100_continous_tubes.png "0_100_continous_diverging")  

![](./images/0_100_continous_tubes_2D.png  "description")   



![](./images/0_100_continous_diverging.png  "description")   

![](./images/0_100_continous_diverging_2D.png "description")   


### discrete
![](./images/0_100_dicrete_no.png  "description")   

![](./images/0_100_dicrete_no_2D.png  "description")   




![](./images/0_100_dicrete_steps.png "description")   

![](./images/0_100_dicrete_steps_2D.png "description")   


![](./images/0_100_dicrete_tubes.png "0_100_dicrete_diverging")  

![](./images/0_100_dicrete_tubes_2D.png  "description")   



![](./images/0_100_dicrete_diverging.png  "description")   

![](./images/0_100_dicrete_diverging_2D.png "description")   

# How to run the code ?

```
make
```




# Dependencies
* gcc
* hsluv-c
* make
* gnuplot
* bash


# Credits
* function hsluv2rgb from [HSLuv-C](https://github.com/hsluv/hsluv-c) = C implementation of [HSLuv = Human-friendly HSL](https://www.hsluv.org/)
* [1D-RGB-color-gradient](https://github.com/adammaj1/1D-RGB-color-gradient)

# Files

# Licence

[Licence](LICENCE)



# Git 



```git
echo "# hsluv-color-gradient" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:adammaj1/hsluv-color-gradient.git
git push -u origin main
```



```
  git clone git@github.com:adammaj1/hsluv-color-gradient.git
```

Subdirectory

```git
mkdir images
git add *.png
git mv  *.png ./images
git commit -m "move"
git push -u origin main
```
then link the images:

```txt
![](./images/n.png "description") 

```
to overwrite 
```git
git mv -f 
```



local repo: ~/hsluv-c/test/t2

