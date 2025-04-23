# esp32-smart-light
```mermaid
flowchart LR
    A[Dispositivo do Usuário] -->|Wi-Fi| B[Roteador Wi-Fi]
    B -->|Wi-Fi| C[ESP32]
    C -->|Servidor Web| D[Relé]
    D --> E[Lâmpada]
