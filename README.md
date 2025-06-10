flowchart LR
    A[Usuário (Celular/PC)] -->|Wi-Fi| B[Roteador]
    B -->|Wi-Fi| C[ESP32]
    C --> D[Protoboard]
    D --> E1[LED Verde (GPIO2)]
    D --> E2[LED Vermelho (GPIO4)]
