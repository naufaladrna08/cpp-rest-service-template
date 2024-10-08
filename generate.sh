#!/bin/bash

# Generate the code

# Loop through the files in odb/model

for file in $(ls odb/model)
do
  # Get the name of the file without the extension
  name=$(echo $file | cut -d'.' -f1)
  # Generate the code
  # odb -d pgsql --generate-query --generate-schema --output-dir odb/src odb/model/users.hxx
  odb -d pgsql --generate-query --generate-schema --output-dir odb/src odb/model/$file -I ../model
done
