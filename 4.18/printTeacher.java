import java.sql.*;
import java.util.Scanner;

public class printTeacher {
	public static void main(String[] args) {
		//预设的用户id和密码
		String c_user_id = "gaussdb";
		String c_password = "Password@123";

		//a.输入登录ID和密码，打开正确的连接
		System.out.println("请输入ID");
		Scanner in = new Scanner(System.in);
		String user_id = in.next();
		System.out.println("请输入密码");
		String password = in.next();
		try {
			try {
				Class.forName( "org.postgresql.Driver");
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
			while(!user_id.equals(c_user_id) || !password.equals(c_password)) {//若用户名和密码错误，可重新输入
				System.out.println("ID或密码错误");
				System.out.println("请输入ID");
				user_id = in.next();
				System.out.println("请输入密码，输入q退出");
				password = in.next();
				if(password.equals("q"))
					throw new Exception("退出程序！");
			}
			Connection conn = DriverManager.getConnection("jdbc:postgresql://localhost:15432/db2022", user_id, password);
			System.out.println("成功连接数据库！");

			//b.输入一个搜索子字符串，返回姓名包含子串的教师(ID, name)
			Statement stmt = conn.createStatement();
			System.out.println("请输入搜索子串");
			String searchString = in.next();
			String strSelect = "select ID, name from instructor where name like '%" + searchString + "%';";
			ResultSet rset = stmt.executeQuery(strSelect);
			while(!rset.isBeforeFirst()) {//若没有名字匹配的教师，可重新输入
				System.out.println("结果为空，是否继续搜索？y/n");
				char ans = in.next().charAt(0);
				System.out.println(ans);
				if(ans == 'n') {
					break;
				}
				System.out.println("请输入搜索子串");
				searchString = in.next();
				strSelect = "select ID, name from instructor where name like '%" + searchString + "%';";
				rset.close();
				rset = stmt.executeQuery(strSelect);
			}
			while(rset.next()) {
				String id = rset.getString("ID");
				String name = rset.getString("name");
				System.out.printf("(%s, %s)\n", id, name);
			}
			rset.close();

			//c.输入一个ID编号（0~99999），检查是否有对应教师
			int ans_id;
			while(true){
				System.out.println("请输入教师ID（0~99999）");
				ans_id = in.nextInt();
				if(ans_id < 0 || ans_id > 99999){
					System.out.println("id错误，请重新输入");
					continue;
				}
				else
					break;
			}
			String s_id = Integer.toString(ans_id);
			strSelect = "select * from instructor where ID = " + s_id + ";";
			rset = stmt.executeQuery(strSelect);
			if(!rset.isBeforeFirst()) {
				System.out.println("没有这个教师");
				rset.close();
			}
			else {
				//d.若有这个教师，打印教师信息
				rset.close();
				strSelect = "select instructor.dept_name, course_id, title, sec_id, semester, year, count(takes.ID) as tot_enrollment "
						+ "from instructor natural join teaches join takes using(course_id, sec_id, semester, year) join course using(course_id) "
						+ "where instructor.ID = " + s_id + " group by instructor.id, course_id, title, sec_id, semester, year "
								+ "order by dept_name,course_id,year,semester;";
				rset = stmt.executeQuery(strSelect);
				if(!rset.isBeforeFirst()) {
					System.out.println("该教师未教授任何课程");
				}
				else {
					System.out.println("(dept_name, course_id, title, sec_id, semester, year, tot_enrollment)");
					while(rset.next()) {
						String dept_name = rset.getString("dept_name");
						String course_id = rset.getString("course_id");
						String title = rset.getString("title");
						String sec_id = rset.getString("sec_id");
						String semester = rset.getString("semester");
						String year = rset.getString("year");
						int tot_enrollment = rset.getInt("tot_enrollment");
						System.out.printf("(%s, %s, %s, %s, %s, %s, %d)\n",dept_name,course_id,title,sec_id,semester,year,tot_enrollment);
					}
				}
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		} 
		catch (Exception e) {
			e.printStackTrace();
		}
		finally{
			in.close();
		}
	}
}
