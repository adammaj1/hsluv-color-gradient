# save as a plot.gp
# run : gnuplot plot.gp

# Set the output file type
set terminal pngcairo size 780,780

# list of gradient names;  update it maually
# gnuplot array is numbered from 1 to words(array), not like c arrays
# use s.c program to update lists from titles and sGradientJoiningType arrays

titles = "continous discrete"

sGradientJoiningType = "no  steps  tubes  diverging"


sH = "0 60 120 180 240 300 360"


saturation = 100





# length of array titles = nMax, but tex files are numbered from 0 to nMax-1 ( c style)
nMax = words(titles) -1 
jMax = words(sGradientJoiningType) -1 
hMax = words(sH) - 1



#  legend
set key inside bottom center horizontal
set key horizontal 
set key font ",8"

# remove upper and right axis
set border 3 back 
set xtics nomirror out
set ytics nomirror


set xlabel "gradient position"
set ylabel "color channel intensity" 

# adjust y range to make a space for the legend 
set yrange [-0.2:1.1]




# plot nMax images
# https://stackoverflow.com/questions/14946530/loop-structure-inside-gnuplot

do for [n=0:nMax]{

	do for [j=0:jMax] {
		
		do for [h=0:hMax] {
		
			# Set the output file name
			outfile = sprintf('%s_100_%s_%s_2D.png', word(sH, h+1), word(titles,n+1), word(sGradientJoiningType, j+1))
			set output outfile
  	
			# Set the intput file name
			infile = sprintf('%s_100_%s_%s.txt', word(sH, h+1), word(titles,n+1), word(sGradientJoiningType, j+1))
				
				
				
			# title of the image for the array of strings
			sTitle = sprintf(" 2D RGB profiles of the %s colormap with %s gradient for hsl = [%s, 100, l]", word(titles,n+1), word(sGradientJoiningType, j+1),word(sH, h+1))
			set title sTitle

			# Now plot the data with lines and points
			plot infile using 1:2 with lines linecolor rgb 'red' title 'R', '' using 1:3 w lines linecolor rgb 'green' title 'G', '' using 1:4 w lines linecolor rgb 'blue' title 'B', '' using 1:5 w lines linecolor rgb 'black' title 'Y'
				
     		}
	
  
	}
}
