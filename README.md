# sortmon

sortmon is a console program that takes its input, sorts it numerically and
shows the top elements (as many as lines there are in the console). 

This is useful for monitoring the output of a command without having to wait
for it to finish (as a normal 'sort' would).

Example:

  du -m ~ | tee >(sort -g disk\_usage.txt ) | sortmon 
