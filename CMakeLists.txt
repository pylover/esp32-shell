set(sources
  "ush.c"
  "cmdline.c"
  "history.c"
  # "term.c"
)


idf_component_register(
  SRCS "${sources}"
  INCLUDE_DIRS "include"
  REQUIRES elog uaio euart
)


target_compile_options(${COMPONENT_LIB} PRIVATE -fms-extensions)

