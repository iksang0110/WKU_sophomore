package com.inventory.network;

import com.inventory.model.Product;
import com.inventory.manager.InventoryManager;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;

public class InventoryServer {
    private static final int PORT = 12345;
    private InventoryManager manager;

    public InventoryServer(InventoryManager manager) {
        this.manager = manager;
    }

    public void startServer() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("서버가 시작되었습니다. 클라이언트를 기다립니다...");
            while (true) {
                try (Socket clientSocket = serverSocket.accept();
                     ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
                     ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream())) {

                    System.out.println("클라이언트가 연결되었습니다.");

                    List<Product> products = manager.getProducts();
                    out.writeObject(products);

                    System.out.println("상품 목록을 클라이언트로 전송했습니다.");
                }
            }
        } catch (IOException e) {
            System.err.println("서버 오류: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        InventoryManager manager = new InventoryManager();
        InventoryServer server = new InventoryServer(manager);
        server.startServer();
    }
}
