    public void Propagate()
    {
        updated.Clear();

        while (flag.Count > 0)
        {
            Vector3Int coord = flag.Pop();
            updated.Add(coord);

            foreach (var direction in Orientations.OrientationUnitVectors)
            {
                if (direction.Key == EOrientations.NULL) continue;

                Vector3Int neighbourCoord = coord + direction.Value;

                if (wave.ValidCoordinate(neighbourCoord))
                {
                    if (wave.GetDataAt(neighbourCoord).AllowedCount() != 1)
                    {
                        Constrain(neighbourCoord);
                    }
                }
            }
        }
    }
