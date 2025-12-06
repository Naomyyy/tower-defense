cmake_minimum_required(VERSION 3.12) # Mudei para 3.12 para suportar CONFIGURE_DEPENDS
project(tower_defense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Definir pastas
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")

# 2. Encontrar os arquivos .cpp
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

# 3. Encontrar SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 4. CRIAR O EXECUTÁVEL (Isso precisa vir antes dos comandos 'target_')
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 5. Configurar diretórios de inclusão (Onde estão os .hpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})

# 6. Linkar as bibliotecas
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# 7. Copiar Assets (Opcional, mas recomendado para copiar a pasta assets automaticamente)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})