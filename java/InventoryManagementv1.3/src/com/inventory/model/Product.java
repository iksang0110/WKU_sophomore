package com.inventory.model;

public class Product extends AbstractProduct {
    public Product(int productID, String name, double price, int quantity, String category) {
        super(productID, name, price, quantity, category);
    }

    @Override
    public void displayProductInfo() {
        System.out.println("상품 ID: " + getProductID());
        System.out.println("이름 : " + getName());
        System.out.println("가격 : " + getPrice());
        System.out.println("수량 : " + getQuantity());
        System.out.println("카테고리 : " + getCategory());
    }
}
