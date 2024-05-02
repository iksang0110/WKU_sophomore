public class App {
    public static void main(String[] args) throws Exception {
        PlusMinus ob1 = new PlusMinus();
        String ssum, sminus;
        ssum = ob1.plus(50, 30);
        sminus = ob1.minus(50, 30);
        System.out.println(ssum);
        System.out.println(sminus);
    }
}
class PlusMinus {

    int add, sub;
    public String plus(int x, int y){
        add = x + y;
        return "두 수의 합 : " + add;
    }
    public String minus(int x, int y){
        sub = x - y;
        return "두 수의 차 : " + sub; 
    }

}