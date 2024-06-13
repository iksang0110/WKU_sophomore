package com.inventory.model;

public class User implements UserInterface {
    private int userID;
    private String name;
    private String role;
    private String password;

    public User(int userID, String name, String role, String password) {
        this.userID = userID;
        this.name = name;
        this.role = role;
        this.password = password;
    }

    @Override
    public int getUserID() {
        return userID;
    }

    @Override
    public void setUserID(int userID) {
        this.userID = userID;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String getRole() {
        return role;
    }

    @Override
    public void setRole(String role) {
        this.role = role;
    }

    @Override
    public String getPassword() {
        return password;
    }

    @Override
    public void setPassword(String password) {
        this.password = password;
    }
}
