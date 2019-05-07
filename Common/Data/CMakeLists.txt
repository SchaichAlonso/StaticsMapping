project (SharedData)

set(my_files
  earthmap1k.jpg
  data.json
  obj8-lights.json
  obj8-frag.glsl
  obj8-vert.glsl
)

foreach (file IN LISTS my_files)
  configure_file(${file} ${file} COPYONLY)
  install(FILES ${file} DESTINATION ${SHARED_DATA_PATH})
endforeach()
