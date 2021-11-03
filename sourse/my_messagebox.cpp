#include "header/my_messagebox.h"

/*********************************************************************
 * @fn      Make_Message_Box_Fail
 *
 * @brief    make message fail message box
 *
 * @param  message - display contents
 *
 * @return   none
 */
void My_messagebox::Make_Message_Box_Fail(QString Message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Fail");
    msgBox.setText(Message);
    msgBox.exec();
}
/*********************************************************************
 * @fn      Make_Message_Box
 *
 * @brief   make Message Box
 *
 * @param  message - display contents
 *              Title - message box title
 *
 * @return   none
 */
void My_messagebox::Make_Message_Box(QString Message,QString Title)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(Title);
    msgBox.setText(Message);
    msgBox.exec();
}
