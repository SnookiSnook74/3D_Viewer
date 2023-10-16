#include "test.h"

START_TEST(test_1) {
  Model* model;
  int status = _get_model("./Model/cube.obj", &model);
  ck_assert_int_eq(status, OK);
  ck_assert_int_eq(model->num_of_faces, 6);
  ck_assert_int_eq(model->total_vertexes, 8);
  ck_assert_double_eq_tol(model->vertexes[0].x, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[0].y, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[0].z, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[1].x, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[1].y, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[1].z, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[2].x, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[2].y, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[2].z, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[3].x, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[3].y, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[3].z, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[4].x, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[4].y, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[4].z, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[5].x, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[5].y, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[5].z, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[6].x, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[6].y, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[6].z, 1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[7].x, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[7].y, -1, 1e-5);
  ck_assert_double_eq_tol(model->vertexes[7].z, 1, 1e-5);
  Vertex vertex1 = model->vertexes[0];
  vertex1 = rotateX(vertex1, 90);
  ck_assert_double_eq_tol(vertex1.x, 1, 1e-5);
  ck_assert_double_eq_tol(vertex1.y, 0.445923, 1e-5);
  ck_assert_double_eq_tol(vertex1.z, 1.34207, 1e-5);
  Vertex vertex2 = model->vertexes[1];
  vertex2 = rotateY(vertex2, 90);
  ck_assert_double_eq_tol(vertex2.x, -1.34207, 1e-5);
  ck_assert_double_eq_tol(vertex2.y, -1, 1e-5);
  ck_assert_double_eq_tol(vertex2.z, -0.445923, 1e-5);
  Vertex vertex3 = model->vertexes[2];
  vertex3 = rotateZ(vertex3, 90);
  ck_assert_double_eq_tol(vertex3.x, -1.34207, 1e-5);
  ck_assert_double_eq_tol(vertex3.y, 0.445923, 1e-5);
  ck_assert_double_eq_tol(vertex3.z, 1, 1e-5);
  Vertex vertex4 = model->vertexes[3];
  vertex4 = scale_vertex(vertex4, 0.9);
  ck_assert_double_eq_tol(vertex4.x, 0.9, 1e-5);
  ck_assert_double_eq_tol(vertex4.y, -0.9, 1e-5);
  ck_assert_double_eq_tol(vertex4.z, 0.9, 1e-5);
  Vertex vertex5 = model->vertexes[4];
  vertex5 = translateX(vertex5, 1);
  ck_assert_double_eq_tol(vertex5.x, 0, 1e-5);
  ck_assert_double_eq_tol(vertex5.y, 1, 1e-5);
  ck_assert_double_eq_tol(vertex5.z, -1, 1e-5);
  Vertex vertex6 = model->vertexes[5];
  vertex6 = translateY(vertex6, 1);
  ck_assert_double_eq_tol(vertex6.x, -1, 1e-5);
  ck_assert_double_eq_tol(vertex6.y, 0, 1e-5);
  ck_assert_double_eq_tol(vertex6.z, -1, 1e-5);
  Vertex vertex7 = model->vertexes[6];
  vertex7 = translateZ(vertex7, 1);
  ck_assert_double_eq_tol(vertex7.x, -1, 1e-5);
  ck_assert_double_eq_tol(vertex7.y, 1, 1e-5);
  ck_assert_double_eq_tol(vertex7.z, 2, 1e-5);
  _reset_model(&model);
  _reset_model(NULL);
}
END_TEST

int main() {
  int number_failed;
  Suite* s = suite_create("back_test");
  TCase* tc = tcase_create("back_test");
  tcase_add_test(tc, test_1);
  suite_add_tcase(s, tc);
  SRunner* sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed == 0 ? 0 : 1;
}