add_llvm_library( BBOpts MODULE BUILDTREE_ONLY
  BBOpts.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
