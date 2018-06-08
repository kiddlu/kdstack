if [ "x$SHELL" != "x/bin/bash" ]; then
     case `ps -o command -p $$` in
         *bash*)
             ;;
         *)
             echo "WARNING: Only bash is supported, use of other shell would lead to erroneous results"
             ;;
     esac
fi