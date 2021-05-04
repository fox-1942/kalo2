#include "scene.h"

void set_callbacks() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_handler);
    glutKeyboardUpFunc(key_up_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutIdleFunc(idle);
    glutSpecialFunc(specialFunc);
    glutTimerFunc(0, set_satellite_led_working_time, 0);
    if (FULLSCREEN == 1) { glutFullScreen(); }
}

void initialize() {
    scene.action.rotate_planets_in_galaxy = true;
    scene.camera.camera_speed = 70;
    init_controller(&scene.move);
    init_draw(&scene.rotate);
    set_callbacks();
    init_camera(&scene.camera);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    scene.action.help = load_texture("..//textures//help.png");
    init_entities(&scene.world);
    glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    if (RESOLUTION == 1) { glutInitWindowSize(1920, 1080); }
    else { glutInitWindowSize(1366, 768); }

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int wnd = glutCreateWindow("New Solar System");
    glutSetWindow(wnd);
    initialize();
    glutMainLoop();
    return 0;
}