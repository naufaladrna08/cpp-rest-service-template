#!/bin/bash

# Generate the code
odb -d pgsql --generate-query --generate-schema --output-dir odb/src odb/model/users.hxx