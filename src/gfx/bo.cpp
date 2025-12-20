#include "BO.h"

// Explicit instantiations (optional if header-only)
template struct BO<BufferType::VBO>;
template struct BO<BufferType::EBO>;
template struct BO<BufferType::UBO>;
