# Dockerfile for testing Dobble app build on openSUSE Leap 15.6
FROM opensuse/leap:15.6

# Set working directory
WORKDIR /app

# Install system dependencies
RUN zypper refresh && \
    zypper install -y \
    # Build tools - need GCC 13+ for C++17 support
    gcc13 \
    gcc13-c++ \
    make \
    cmake \
    git \
    # Qt6 dependencies
    qt6-base-devel \
    qt6-base-common-devel \
    qt6-webenginecore-devel \
    qt6-webenginewidgets-devel \
    libatomic1 \
    libQt6Core6 \
    libQt6Widgets6 \
    libQt6WebEngineCore6 \
    libQt6WebEngineWidgets6 \
    libQt6Gui6 \
    libQt6DBus6 \
    libQt6Network6 \
    # Node.js for web UI
    nodejs20 \
    npm20 \
    # Threading support
    glibc-devel && \
    zypper clean -a

# Set GCC 13 as default (required for C++17 features like <filesystem>)
ENV CC=/usr/bin/gcc-13
ENV CXX=/usr/bin/g++-13

# Copy project files
COPY . .

# Build the web client first
WORKDIR /app/ui
RUN npm install && \
    npm run build

# Configure and build the C++ components
WORKDIR /app
RUN cmake -S . -B build && \
    cmake --build build

# Verify all targets were built
RUN ls -lh build/dobble_client build/dobble_gameserver build/dobble_lobbyserver

# Set the default command to show build info
CMD ["sh", "-c", "echo 'Build completed successfully!' && ls -lh build/dobble_*"]
