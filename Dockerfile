# ---------- Build Stage ----------
FROM gcc:11 AS builder

WORKDIR /app

# Install build tools
RUN apt-get update && apt-get install -y cmake

# Copy source
COPY . .

# Build the project
RUN cmake -S . -B build \
    && cmake --build build

# ---------- Runtime Stage ----------
FROM debian:bookworm-slim

WORKDIR /app

# Copy binary
COPY --from=builder /app/build/inventory_app /app/inventory_app

# Copy static files & DB
COPY public /app/public
COPY db /app/db

# Expose port
EXPOSE 8080

# Run the app
CMD ["./inventory_app"]
