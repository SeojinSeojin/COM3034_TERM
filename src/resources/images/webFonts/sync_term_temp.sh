# iterate over all directories
for dir in */ ; do
    # copy the contents of each directory to the corresponding directory in f2
    rsync -av --ignore-existing "$dir"/ ../../term_temp/lower_temp/"$dir"/
done