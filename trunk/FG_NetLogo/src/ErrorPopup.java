/*
 *  Written By Elias Jordan, 2009
 */

import javax.swing.*;

/* Very basic dialog box that does not require a parent content item to be
 * specified as the one built into swing does. This is so it can be eaisly used
 * in an extension.
 */
public class ErrorPopup
{
	public static void show(String title, String message)
	{
		JFrame frame = new JFrame();
		
		frame.setTitle(title);
		JPanel panel = new JPanel();
		JLabel label = new JLabel("<html><b><font size=6 color=red>" + title + "</font></b><br><br>" + message + "</html>");
		panel.add(label);
		
		frame.setContentPane(panel);
		frame.pack();
		frame.setVisible(true);
	}
}
