void AWFC::Propagate() {
	SCOPE_CYCLE_COUNTER( STAT_Propagate );

	Updated.Empty();

	while ( Flag.Num() > 0 ) {
		FIntVector Coord = Flag.Pop();
		Updated.Add( Coord );

		for ( auto& Direction : UOrientations::OrientationUnitVectors ) {
			if ( Direction.Key == EOrientations::NONE ) continue;

			const FIntVector NeighbourCoord = Coord + Direction.Value;

			if ( Wave.IsValidCoordinate( NeighbourCoord ) ) {
				if ( Wave.GetDataAt( NeighbourCoord ).AllowedCount() != 1 ) {
					Constrain( NeighbourCoord );
				}
			}
		}
	}
}
