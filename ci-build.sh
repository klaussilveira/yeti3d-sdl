#!/bin/sh

failed=0;

# Default Build
(make clean yeti) || failed=1;

if [ $failed -eq 1 ]; then
    echo "Build failure.";
else
    echo "All builds successful.";
fi

exit $failed;
