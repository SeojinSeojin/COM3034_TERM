# iterate over all directories
for dir in */ ; do
    # copy the contents of each directory to the corresponding directory in f2
    rsync -av --ignore-existing "$dir"/ /Users/kimseojin/Desktop/이번학기/지능형비전/more_0531/5_renamed/"$dir"/
done