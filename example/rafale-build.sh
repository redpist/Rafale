#!/bin/sh

###################################################################
# This wrapper emulate a "clean" FCGI generation behavior         #
# 1/ copy the sources to a 'build' dir                            #
# 2/ make sure rafale "generated/models" exists                   #
# 3/ call rafale_generator on the build dir                       #
# 4/ move generated FCGI to project root dir                      #
###################################################################

RAFALE_GENERATOR=rafale_generator
SOURCE_DIR=project
BUILD_DIR=build
FCGI_DESTINATION=.

echo "########################### Staging project into build ###########################"
rm -rf "$BUILD_DIR"
cp -r "$SOURCE_DIR" "$BUILD_DIR"
mkdir -p "$BUILD_DIR/generated/models"
echo "############################### Generating project ###############################"
cd "$BUILD_DIR"
$RAFALE_GENERATOR .
make
cd -
echo "################################### Moving FCGI ##################################"
mv "$BUILD_DIR"/public/index.fcgi rafale.fcgi

