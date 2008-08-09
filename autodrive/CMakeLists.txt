PROJECT (autodrive)

  ADD_EXECUTABLE(autodrive
			opengl_3d.cpp
			main.cpp
			grid_pdf.cpp
			move_vehicles.cpp
			object_data.cpp
			space.cpp
			gnuplot_i.cc
			fgcontroller.cpp
		)
  ADD_EXECUTABLE(fgcontrol fgfsclient.cxx)
  ADD_EXECUTABLE(setvelocity setvelocity.cpp)
  ADD_EXECUTABLE(followbehind followbehind.cpp)
			
  SET(AUTODRIVE_LIBS lapack gsl glut gslcblas stdc++)

  SET(FGCONTROL_LIBS stdc++)  
  SET(SETVELOCITY_LIBS stdc++)  
  SET(FOLLOWBEHIND_LIBS stdc++)  

  TARGET_LINK_LIBRARIES( autodrive ${AUTODRIVE_LIBS})
  TARGET_LINK_LIBRARIES( fgcontrol ${FGCONTROL_LIBS})
  TARGET_LINK_LIBRARIES( setvelocity ${SETVELOCITY_LIBS})
  TARGET_LINK_LIBRARIES( followbehind ${FOLLOWBEHIND_LIBS})
    
  LINK_DIRECTORIES(/usr/local/lib /usr/lib/atlas)

      
      
      
