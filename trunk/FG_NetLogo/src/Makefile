uni:
	javac -classpath NetLogo.jar:./ FGWaypointExtension.java
	jar cvfm fginterface.jar manifest.txt ./*.class
	
	cp ./fginterface.jar ../netlogo-4.0.4/extensions/fginterface

	rm ./*.class

home:	
	javac -classpath ../NetLogo4.1/NetLogo.jar:./ FGWaypointExtension.java
	jar cvfm fginterface.jar manifest.txt ./*.class
	
	cp ./fginterface.jar ../NetLogo4.1/extensions/fginterface/
	rm ../NetLogo4.1/extensions/fginterface/fginterface.jar
	cp ./fginterface.jar ../NetLogo4.1/extensions/fginterface/
	cp ./FGInterface.properties ../NetLogo4.1/extensions/fginterface/

	rm ./*.class


