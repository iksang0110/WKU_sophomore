package com.inventory.network;

import com.inventory.model.Product;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.Socket;
import java.util.List;

public class InventoryClient {
    private static final String SERVER_ADDRESS = "localhost";
    private static final int PORT = 12345;

    public void connectToServer() {
        try (Socket socket = new Socket(SERVER_ADDRESS, PORT);
             ObjectInputStream in = new ObjectInputStream(socket.getInputStream())) {

            List<Product> products = (List<Product>) in.readObject();
            System.out.println("서버로부터 상품 목록을 받았습니다:");
            for (Product product : products) {
                System.out.println("┌────────────────────────────┐");
                System.out.println("│ 상품 ID: " + product.getProductID());
                System.out.println("│ 상품명: " + product.getName());
                System.out.println("│ 가격: " + product.getPrice());
                System.out.println("│ 수량: " + product.getQuantity());
                System.out.println("│ 카테고리: " + product.getCategory());
                System.out.println("└────────────────────────────┘");
            }
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("클라이언트 오류: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        InventoryClient client = new InventoryClient();
        client.connectToServer();
    }
}
