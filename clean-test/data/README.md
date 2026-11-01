# Data provenance

- `example.stl`: self-created ASCII STL, a 1 m cube (12 triangles, coordinates in metres). Not a CAD export, not licensed from a third party.
- `ansys_temperature_synthetic.csv`: SYNTHETIC, self-created, labelled as such in the UI and here. It is NOT an actual ANSYS export. Schema: node_id,x_m,y_m,z_m,temperature_C. Units: metres and degrees Celsius. Values follow the Task C exact solution along x (100, 88.75, 105, 93.75, 20 C at x = 0, 0.25, 0.5, 0.75, 1 m) so the point cloud can be cross-checked against the FEA curve.
- Tested limits (this prototype): STL up to a few thousand triangles and CSV up to a few thousand nodes load instantly in the UI; the C++ parsers have no hard size cap but are not benchmarked beyond that, which is stated as a limitation in the main README.
- Malformed files: the C++ parsers reject a missing `solid`/`endsolid`, a facet without 3 vertices, a wrong CSV header, a wrong column count, non-numeric or non-finite values, and an empty file, each with a line-specific message.
