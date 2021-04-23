powershell -command "Expand-Archive external\wxWidgets-3.1.5\vc_lib.zip external\wxWidgets-3.1.5\vc_lib"
MOVE "external\wxWidgets-3.1.5\vc_lib\vc_lib\mswu" "external\wxWidgets-3.1.5\vc_lib\mswu"
MOVE "external\wxWidgets-3.1.5\vc_lib\vc_lib\mswud" "external\wxWidgets-3.1.5\vc_lib\mswud"
MOVE "external\wxWidgets-3.1.5\vc_lib\vc_lib\*.*" "external\wxWidgets-3.1.5\vc_lib"